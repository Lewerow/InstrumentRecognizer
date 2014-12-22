@echo off
set REPORT_DIR=data\wine\reports_cv%3\%2
mkdir %REPORT_DIR%
Recognizer.exe --step=all --data_file=data\wine\wine.csv --class_name_column=0 --description_dir=data\wine\descriptions --reports_dir=%REPORT_DIR% --fold_count=%3 --classifier_type=%1_%2