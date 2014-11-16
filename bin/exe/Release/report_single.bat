grep "time" summary.txt | sed "s/.*reports_cv\([0-9]*\).*/\1/g" > crossvalidations.txt
grep "time" summary.txt | sed "s/.*Equal\(.*\)_.*\/Summary.*/\1/g" > types.txt
grep "time" summary.txt | sed "s/.*Equal.*_\(.*\)\/Summary.*/\1/g" > counts.txt
grep "time" summary.txt | sed "s/.*time: \([0-9]*\) milliseconds/\1/g" > times.txt
grep "accurracy" summary.txt | sed "s/.*accurracy: \(.*\)/\1/g" > accurracies.txt
grep "recall" summary.txt | sed "s/.*recall: \(.*\)/\1/g" > recalls.txt
grep "precision" summary.txt | sed "s/.*precision: \(.*\)/\1/g"  > precisions.txt
grep "F\-measure" summary.txt | sed "s/.*F\-measure: \(.*\)/\1/g"  > fmeasures.txt

echo Typ dyskretyzacji	Liczba klas w ramach atrybutu	Liczba foldów	Średnia wartość parametru accurracy	Średnia wartość parametru recall	Średnia wartość parametru precision	Średnia wartość parametru F-score	Całkowity czas uczenia >tab.txt
paste types.txt counts.txt crossvalidations.txt accurracies.txt recalls.txt precisions.txt fmeasures.txt times.txt | sed "s/\r\t/\t/g" >>tab.txt