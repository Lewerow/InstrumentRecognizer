@echo off

set REPORT_ROOT=data\glass\reports_cv%3
mkdir %REPORT_ROOT%
set REPORT_DIR=%REPORT_ROOT%\%2
mkdir %REPORT_DIR%
Recognizer.exe --step=all --data_file=data\glass\glass.csv --class_name_column=9 --description_dir=data\glass\descriptions  --reports_dir=%REPORT_DIR% --fold_count=%3 --classifier_type=%1_%2