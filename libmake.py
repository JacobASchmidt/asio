import json
import os


def name(f):
    return f.name[:f.name.rindex(".")]


def extension(f):
    return f.name[f.name.rindex("."):]


class File:
    def __init__(self, *, directory, name, extension, dependencies=[], make=""):
        self.directory = directory
        self.name = name
        self.extension = extension
        self.dependencies = dependencies
        self.make = make

    def path(self, joiner="/"):
        return f"{self.directory}/{self.name}{self.extension}"

    def __repr__(self):
        return f"File{{directory='{self.directory}', name='{self.name}', exstention='{self.extension}'', dependencies={ self.dependencies}, make='{self.make}'"


MAKE_JSON = ".make.json"


def get_info():
    with open(MAKE_JSON, "r+") as f:
        lines = f.readlines()
        s = "".join(lines)
        if len(s) == 0:
            enc = json.JSONEncoder()
            s = enc.encode({})
            f.write(s)
            return {}
        dec = json.JSONDecoder()
        ret = dec.decode("".join(lines))
    return ret


def set_info(info):
    prev = get_info()
    prev = dict(prev,  **info)
    with open(MAKE_JSON, "w") as f:
        f.write(json.JSONEncoder().encode(prev))


def get_last_use(f):
    fd = os.open(f.path(), os.O_RDONLY)
    res = os.fstat(fd)
    os.close(fd)
    return res.st_mtime_ns


def make_impl(f, info):
    should_update = False
    should_update = any([make_impl(dep, info) for dep in f.dependencies])
    try:
        time = get_last_use(f)
    except:
        should_update = True
    if not should_update:
        if f.path() in info:
            last_known = info[f.path()]
            if last_known < time:
                should_update = True
        else:
            should_update = True
    if should_update:
        if f.make:
            if type(f.make) == str:
                cmd = f.make
            else:
                cmd = f.make(f)

            print(cmd)
            os.system(cmd)

    info[f.path()] = get_last_use(f)
    return should_update


def make(*args):
    info = get_info()
    for arg in args:
        make_impl(arg, info)
    set_info(info)
