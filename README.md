# Maxcut Setting Framework

This framework was used in my Master's Thesis, where it is referred to as the maxcut-setting.

## Building

The repository uses submodules for dependencies, so run `git submodule init %% git submodule update` before the first build.

From top level folder:
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Running the example configuration

Assuming you have an instance file in a supported file format under `~/instance.rud`

From top level folder:
```bash
mkdir run
cd run
ln -s ../build/maxcut-framework
cp ../config.json .
./maxcut-framework ~/instance.rud config.json

