import os
import tarfile

WORKPATH = os.path.dirname(os.path.realpath(__file__))


def read_yaml(stream):
    # Alternative to yaml.safe_load
    result = {}
    current_section = None
    for line in stream.readlines():
        line = line.strip()
        if line.strip().startswith("-"):
            result.setdefault(current_section, []).append(line[1:].strip())
        else:
            current_section = line.strip(": ")
            result.setdefault(current_section, [])
    return result


def packaged_sources(distfiles_yml):
    ymls = [distfiles_yml]
    all_sources = []
    while ymls:
        yml = ymls.pop()
        content = read_yaml(open(yml))
        all_sources.extend([os.path.join(os.path.dirname(yml), f) for f in content.get("files", [])])
        ymls.extend([os.path.join(os.path.dirname(yml), d, "distfiles.yml") for d in content.get("subdirs", [])])
    return all_sources


version = open(os.path.join(WORKPATH, "NAIVETSEARCH_VERSION")).read().strip()

tar_filebasename = "naive-tsearch-{}".format(version)
tar_compressions = ("gz", "xz")
for tar_compression in tar_compressions:
    tar_filename = "{}.tar.{}".format(tar_filebasename, tar_compression)
    print("Creating {}...".format(tar_filename))
    with tarfile.open(tar_filename, mode="w:{}".format(tar_compression)) as tar:
        for file in packaged_sources(os.path.join(WORKPATH, "distfiles.yml")):
            name = os.path.join(tar_filebasename, os.path.relpath(file, WORKPATH))
            print("Adding {}...".format(name))
            tar.add(name=file, arcname=name)
