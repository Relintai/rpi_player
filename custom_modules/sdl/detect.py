import os
import platform
import sys


def is_active():
    return True


def get_name():
    return "sdl2"


def can_build():
    if os.name == "posix" or sys.platform == "darwin":
        x11_error = os.system("pkg-config --version > /dev/null")
        if x11_error:
            return False

        libevent_err = os.system("pkg-config sdl2 --modversion --silence-errors > /dev/null ")

        if libevent_err:
            print("sdl2 not found!")
            return False

        print("sdl2 found!")

        return True

    return False


def get_opts():
    return []

def get_flags():

    return []


def configure(env):
    env.ParseConfig("pkg-config sdl2 --cflags --libs")
    
    env.Append(CPPDEFINES=["SDL_PRESENT"])


