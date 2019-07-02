@set PATH=%PATH%;%MINGW_BIN%

cd data
@call renderdoccmd capture -d . "../bin/scratch.exe"
cd ..