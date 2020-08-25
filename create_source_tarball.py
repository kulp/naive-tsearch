from pathlib import Path
import tarfile
import yaml

WORKPATH = Path(__file__).resolve().parent


def packaged_sources(distfiles_yml: Path):
    ymls = [distfiles_yml]
    all_sources = []
    while ymls:
        yml = ymls.pop()
        content = yaml.safe_load(yml.open())
        all_sources.extend([yml.parent /  f for f in content.get("files", [])])
        ymls.extend([yml.parent / d / "distfiles.yml" for d in content.get("subdirs", [])])
    return all_sources


version = (WORKPATH / "NAIVETSEARCH_VERSION").read_text().strip()

tar_filebasename = "naive-tsearch-{}".format(version)
tar_compressions = ("gz", "xz")
for tar_compression in tar_compressions:
    tar_filename = "{}.tar.{}".format(tar_filebasename, tar_compression)
    print("Creating {}...".format(tar_filename))
    with tarfile.open(tar_filename, mode="w:{}".format(tar_compression)) as tar:
        for file in packaged_sources(WORKPATH / "distfiles.yml"):
            name = Path(tar_filebasename) / file.relative_to(WORKPATH)
            print("Adding {}...".format(name))
            tar.add(name=file, arcname=str(name))


# Zero the embedded timestamp in the gzip'ed tarball
with open("naive-tsearch-{}.tar.gz".format(version), "r+b") as f:
    f.seek(4, 0)
    f.write(b"\x00\x00\x00\x00")
