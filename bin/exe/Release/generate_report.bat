mkdir summaries
grep -nre "Mean\|Training time" data | grep "Summary" > summaries/summaries.txt

cd summaries

mkdir glass
mkdir wine
mkdir iris

grep "glass" summaries.txt > glass/summary.txt
grep "wine" summaries.txt > wine/summary.txt
grep "iris" summaries.txt > iris/summary.txt

cd glass
call ../../report_single.bat

cd ../wine
call ../../report_single.bat

cd ../iris
call ../../report_single.bat

cd ../..