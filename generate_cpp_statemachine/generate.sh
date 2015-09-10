(
export PYTHONPATH=$PYTHONPATH:../../stateproto/src/extensions/lang/python/
python3 generate_cpp.py  --input_filename "$1" --cpp_output_filename "$1".cpp
)
less "$1".cpp

