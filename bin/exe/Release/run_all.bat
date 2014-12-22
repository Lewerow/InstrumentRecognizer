@echo off
call run_knn.bat data\glass\glass.csv 9 data\glass\descriptions data\glass
call run_knn.bat data\wine\wine.csv 0 data\wine\descriptions data\wine
call run_knn.bat data\iris\iris.csv 4 data\iris\descriptions data\iris
call run_knn.bat data\seeds\seeds.csv 8 data\seeds\descriptions data\seeds
call run_knn.bat data\diabetes\diabetes.csv 9 data\diabetes\descriptions data\diabetes
