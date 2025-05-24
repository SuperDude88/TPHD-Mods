# Aesthetic Armor
A mod that makes Twilight Princess HD's magic armor behave like a double defense upgrade, without draining your rupees.

## Installation
(`[ENVIRONMENT]` is a placeholder for the actual environment name.)

1. Extract the desired `.wps` files into `sd:/wiiu/environments/[ENVIRONMENT]/plugins`.
2. Requires the [WiiUPluginLoaderBackend](https://github.com/wiiu-env/WiiUPluginLoaderBackend) in `sd:/wiiu/environments/[ENVIRONMENT]/modules`.
3. Requires the [KernelModule](https://github.com/wiiu-env/KernelModule) to be running via [WUMSLoader](https://github.com/wiiu-env/WUMSLoader).

## Building

For building you need:

- [wut](https://github.com/devkitPro/wut)
- [wups](https://github.com/wiiu-env/WiiUPluginSystem)
- [libkernel](https://github.com/wiiu-env/libkernel)

This can be set up automatically using the Dockerfile:
```
# Build docker image (only needed once)
docker build . -t mod-builder

# Compile to .wps
docker run -v "$(pwd):/src" mod-builder
```
