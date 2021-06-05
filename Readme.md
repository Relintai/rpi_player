# Data collection test app for rcpp framework

Simple data collector app written with rcpp framework for a university project.

## Compilation

Will only work on linux! Works on the rasberry pi.

### Dependencies

Arch/Manjaro:

``` 
pacman -S --needed scons pkgconf gcc yasm libevent
```

Debian/Raspian:

```
sudo apt-get install build-essential scons pkg-config libudev-dev yasm libevent libevent-dev
```

Optionally if you install MariaDB/MySQL and/or PostgreSQL the compile system should pick it up. Make sure to get a version
whoch contains the development headers (A bunch of .h files).

### Initial setup

clone this repo, then call `scons`, it will clone rcpp cms into a new engine directory. Run this every time you update the project.
You don't have to run it before / between builds.

```
# git clone https://github.com/Relintai/rcpp_sample_simple_data_collector_app.git rcpp_sample_simple_data_collector_app
# cd school_industrial_comm_2021
# scons
```

Now you can build the project like: `scons bl`.  ([b]uild [l]inux)

Adding -jX to the build command will run the build on that many threads. Like: `scons bl -j4`.

```
# scons bl -j4
- or -
# ./build.sh
```
Now you can run it.

First run migrations, this will create the necessary database tables:

```
# ./engine/bin/server m
- or -
# ./migrate.sh
```

Now you can start the server:

```
# ./engine/bin/server
- or -
# ./run.sh
```

Make sure to run it from the project's directory, as it needs data files.

Now just open http://127.0.0.1:8080

You can push floats to the "a/b" MQTT topics, and the new values will be save in the `database.sqlite` file, and will appear
in your browser.

If you have mosqitto installed you can use the `publish_random_data.py` or `publish_data.sh` scripts to automatically
push things into the proper MQTT topic.

## Structure

The main Application implementation is `app/ic_application.h`.

The `main.cpp` contains the initialization code for the framework.

The `content/www` folder is the wwwroot.
