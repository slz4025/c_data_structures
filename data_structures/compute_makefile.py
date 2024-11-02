from dataclasses import dataclass
import glob
import os
import re
from typing import Optional

makefile_path = "Makefile"
target = "data_structures.a"
cc = "egcc"
# -Wall shows all warnings
# -lpthread imports pthread library
cflags = "-Wall -lpthread"

"""
Stores all c files.
"""
src_dir = "src"

"""
Stores all internal header files.
"""
include_dir = "include"

"""
TODO: Implement.
Stores all external header files and static libraries.
"""
lib_dir = "lib"

"""
Stores all object files.
"""
bin_dir = "bin"

"""
Stores all test executables.
"""
test_dir = "test"


@dataclass
class ObjectFile:
    o_path: str
    c_path: str
    referenced_deps: set[str]
    all_deps: set[str]
    test_target: Optional[str]


def get_o_from_c_path(c_path):
    pre, ext = os.path.splitext(c_path)
    o_path = os.path.join(
        bin_dir,
        pre.removeprefix(src_dir + "/").replace("/", "__") + ".o"
    )
    return o_path


def get_h_from_c_path(c_path):
    pre, ext = os.path.splitext(c_path)
    h_path = os.path.join(
        include_dir,
        pre.removeprefix(src_dir + "/").replace("/", "__") + ".h"
    )
    return h_path


def get_o_from_h_path(h_path):
    pre, ext = os.path.splitext(h_path)
    o_path = os.path.join(
        bin_dir,
        pre.removeprefix(include_dir + "/").replace("/", "__") + ".o"
    )
    return o_path


def get_header_paths(path):
    if not os.path.exists(path):
        return set()

    include_header_pattern = r"#include \"(?P<header_path>.*)\""
    path_dir = os.path.dirname(path)

    header_paths = set()
    f = open(path, "r")
    lines = f.readlines()
    for line in lines:
        m = re.match(include_header_pattern, line)
        if m is None:
            continue
        header_path = m.group(1)
        base_header_path = os.path.normpath(
            os.path.join(path_dir, header_path)
        )
        header_paths.add(base_header_path)

    return header_paths


def get_referenced_dependencies(c_path):
    header_paths_from_c = get_header_paths(c_path)
    h_path = get_h_from_c_path(c_path)
    header_paths_from_h = get_header_paths(h_path)
    header_paths = header_paths_from_c.union(header_paths_from_h)
    referenced_deps = set()
    for header_path in header_paths:
        o_path = get_o_from_h_path(header_path)
        referenced_deps.add(o_path)
    return referenced_deps


def create_object_file(c_path):
    o_path = get_o_from_c_path(c_path)
    referenced_deps = get_referenced_dependencies(c_path)

    test_target = None
    if c_path.endswith(".test.c"):
        test_target = test_dir + "/" \
            + c_path \
            .removeprefix(src_dir + "/") \
            .removesuffix(".test.c")

    object_file = ObjectFile(
        o_path=o_path,
        c_path=c_path,
        referenced_deps=referenced_deps,
        all_deps=[],  # defined later
        test_target=test_target,
    )
    return object_file


def get_all_dependencies(object_file, object_files):
    """
    Get graph of object dependencies.
    """
    all_dependencies = set()
    unvisited = set(object_file.referenced_deps)
    visited = set()
    while len(unvisited) > 0:
        dep = unvisited.pop()
        visited.add(dep)

        if dep == object_file.o_path:
            continue
        if dep not in object_files:
            continue
        dep_object_file = object_files[dep]

        unvisited = unvisited.union(
            dep_object_file.referenced_deps.difference(visited)
        )
        all_dependencies.add(dep)
    return all_dependencies


def create_rules():
    """
    Create a rule to make an object file for each c file found.
    """
    c_file_pattern = f"{src_dir}/**/*.c"
    c_paths = glob.glob(c_file_pattern, recursive=True)

    object_files = {}
    for c_path in c_paths:
        object_file = create_object_file(c_path)
        object_files[object_file.o_path] = object_file

    for object_file in object_files.values():
        all_dependencies = get_all_dependencies(object_file, object_files)
        object_file.all_deps = all_dependencies

    object_rules = {}
    for object_file in object_files.values():
        object_rule = f"""
{object_file.o_path}: {object_file.c_path}
\t$(CC) -o {object_file.o_path} -c {object_file.c_path}
"""
        object_rules[object_file.o_path] = object_rule

    test_rules = {}
    for object_file in object_files.values():
        if object_file.test_target is None:
            continue
        dep_paths = " ".join(object_file.all_deps)
        test_rule = f"""
{object_file.test_target}: {object_file.o_path} {dep_paths}
\t$(CC) $(CFLAGS) {object_file.o_path} {dep_paths} -o {object_file.test_target}
"""
        test_rules[object_file.test_target] = test_rule

    target_deps = []
    for object_target, object_file in object_files.items():
        if object_file.test_target is not None:
            continue
        target_deps.append(object_target)
    target_dep_paths = " ".join(target_deps)
    default_rule = f"""
$(TARGET): {target_dep_paths}
\tar -r $(TARGET) {target_dep_paths}
"""

    obj_targets = " ".join(object_rules.keys())
    test_targets = " ".join(test_rules.keys())
    all_rule = f"""
all: {obj_targets} {test_targets} $(TARGET)
"""

    return all_rule \
        + default_rule \
        + "".join(test_rules.values()) \
        + "".join(object_rules.values())


def create_makefile():
    targets = create_rules()

    preamble = f"""
CC= {cc}
CFLAGS= {cflags}
TARGET= {target}
"""

    epilogue = f"""
clean:
\trm -rf {bin_dir}/* {test_dir}/*
"""

    contents = preamble \
        + "\n" + targets \
        + "\n" + epilogue
    with open(makefile_path, '+w') as f:
        f.write(contents)


if __name__ == "__main__":
    create_makefile()
