@echo off
set REPORT_DIR=data\iris\reports_cv%3\%2
mkdir %REPORT_DIR%
Recognizer.exe --step=all --data_file=data\iris\iris.csv --class_name_column=4 --description_dir=data\iris\descriptions --reports_dir=%REPORT_DIR% --fold_count=%3 --classifier_type=%1_%2