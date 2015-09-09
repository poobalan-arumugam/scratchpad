(
export PYTHONPATH=$PYTHONPATH:../../stateproto/src/extensions/lang/python/
python generate_go.py  --input_filename test_machine1.sm1 --golang_output_filename result_test_statemachine1.go-temp
goimports result_test_statemachine1.go-temp > result_test_statemachine1.go
less result_test_statemachine1.go-temp
less result_test_statemachine1.go
)
