# sendevents

`sendevents` is a command line tool for Android platform, written in C language.

## Build and install

This project is build under x86 Linux, for example Ubuntu 16.04 LTS.

### How to setup build environment

```sh
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install gcc
sudo apt-get install gcc-5-arm-linux-gnueabi
sudo apt-get install make
```

### How to build

```sh
make
make test
```

### How to install

Install [Android SDK](https://developer.android.com/studio/index.html) for `adb` command. And connect your device to your PC using USB cable.

```sh
make install
```

## Release History

* 0.1.0
    * The first proper release

## License

Distributed under the MIT license. See `LICENSE.txt` for more information.

