setwd("D:/C++/InstrumentRecognizer/bin/exe/Release")

draw_cv_series <- function(tab, metric, vote){

	metric_vote=tab[tab[1] == metric & tab[2] == vote,]
	metric_vote=metric_vote[order(metric_vote[,3]),]

	metric_vote_cv2 = metric_vote[metric_vote[4] == 2,]
	metric_vote_cv3 = metric_vote[metric_vote[4] == 3,]
	metric_vote_cv5 = metric_vote[metric_vote[4] == 5,]
	metric_vote_cv7 = metric_vote[metric_vote[4] == 7,]
	metric_vote_cv10 = metric_vote[metric_vote[4] == 10,]

	miny = min(metric_vote[,8])*0.9
	maxy = max(metric_vote[,8])*1.1

	minx = min(metric_vote[,3])*0.9
	maxx = max(metric_vote[,3])*1.1

	#FScore
	jpeg(paste(metric, vote, "fscore_vs_neighbors_folds_as_series.jpg"))
	plot(1, 1, xlim=c(minx, maxx), ylim=c(miny, maxy), xlab="Rozmiar sasiedztwa", ylab="F-Score")
	title(main=paste("F-Score w zaleznosci od rozmiaru sasiedztwa dla\n miary odleglosci:", metric, " i glosowania: ", vote))

	points(metric_vote_cv2[,3], metric_vote_cv2[,8], col="blue", pch=19)
	points(metric_vote_cv3[,3], metric_vote_cv3[,8], col="red", pch=20)
	points(metric_vote_cv5[,3], metric_vote_cv5[,8], col="green", pch=21)
	points(metric_vote_cv7[,3], metric_vote_cv7[,8], col="black", pch=22)
	points(metric_vote_cv10[,3], metric_vote_cv10[,8], col="orange",pch=23)

	legend("bottomright", legend=c("2 foldy", "3 foldy", "5 foldow", "7 foldow", "10 foldow"), 
		pch=c(19,20,21,22,23), col=c("blue", "red", "green", "black", "orange"))
	dev.off()
} 

draw_metric_series <- function(tab, vote){

	metric_vote=tab[tab[2] == vote & tab[4] == 10,]
	metric_vote=metric_vote[order(metric_vote[,3]),]

	vote_euclid = metric_vote[metric_vote[1] == "Euclid",]
	vote_manhattan = metric_vote[metric_vote[1] == "Manhattan",]
	vote_minkowski3 = metric_vote[metric_vote[1] == "Minkowski3",]

	miny = min(metric_vote[,8])*0.9
	maxy = max(metric_vote[,8])*1.1

	minx = min(metric_vote[,3])*0.9
	maxx = max(metric_vote[,3])*1.1

	#FScore
	jpeg(paste(metric, vote, "fscore_vs_neighbors_metrics_as_series.jpg"))
	plot(1, 0, xlim=c(minx, maxx), ylim=c(miny, maxy), xlab="Rozmiar sasiedztwa", ylab="F-Score")
	title(main=paste("F-Score w zaleznosci od rozmiaru sasiedztwa dla\n glosowania:", vote, " i 10 foldow walidacji krzyzowej"))

	points(vote_euclid[,3], vote_euclid[,8], col="blue", pch=19)
	points(vote_manhattan[,3], vote_manhattan[,8], col="red", pch=20)
	points(vote_minkowski3[,3], vote_minkowski3[,8], col="green", pch=21)

	legend("bottomright", legend=c("Euklidesowa", "Manhattan", "Minkowskiego"), 
		pch=c(19,20,21), col=c("blue", "red", "green"))
	dev.off()
} 

draw_vote_series <- function(tab, metric){

	metric_vote=tab[tab[1] == metric & tab[4] == 10,]
	metric_vote=metric_vote[order(metric_vote[,3]),]

	metric_majority = metric_vote[metric_vote[2] == "Majority",]
	metric_weighted = metric_vote[metric_vote[2] == "Weighted",]
	metric_dual = metric_vote[metric_vote[2] == "DualWeighted",]

	miny = min(metric_vote[,8])*0.9
	maxy = max(metric_vote[,8])*1.1

	minx = min(metric_vote[,3])*0.9
	maxx = max(metric_vote[,3])*1.1

	#FScore
	jpeg(paste(metric, vote, "fscore_vs_neighbors_metrics_as_series.jpg"))
	plot(1, 0, xlim=c(minx, maxx), ylim=c(miny, maxy), xlab="Rozmiar sasiedztwa", ylab="F-Score")
	title(main=paste("F-Score w zaleznosci od rozmiaru sasiedztwa dla\n miary:", metric, " i 10 foldow walidacji krzyzowej"))

	points(metric_majority[,3], metric_majority[,8], col="blue", pch=19)
	points(metric_weighted[,3], metric_weighted[,8], col="red", pch=20)
	points(metric_dual[,3], metric_dual[,8], col="green", pch=21)

	legend("bottomright", legend=c("Wiekszosciowa", "Wazona", "Podwojnie wazona"), 
		pch=c(19,20,21), col=c("blue", "red", "green"))
	dev.off()
} 

setwd("summaries/glass")
glass=read.csv("tab.txt", TRUE, "\t")

for(metric in c("Euclid", "Manhattan", "Minkowski3"))
{
    for(vote in c("Majority", "Weighted", "DualWeighted"))
	{
        draw_cv_series(glass, metric, vote)
	}
}

setwd("../iris")
iris=read.csv("tab.txt", TRUE, "\t")

for(vote in c("Majority", "Weighted", "DualWeighted"))
{
    draw_metric_series(iris, vote)
}

setwd("../wine")
wine=read.csv("tab.txt", TRUE, "\t")

for(metric in c("Euclid", "Manhattan", "Minkowski3"))
{
    draw_vote_series(wine, metric)
}

setwd("../diabetes")
diabetes=read.csv("tab.txt", TRUE, "\t")

for(vote in c("Majority", "Weighted", "DualWeighted"))
{
    draw_metric_series(diabetes, vote)
}

setwd("../seeds")
diabetes=read.csv("tab.txt", TRUE, "\t")

for(metric in c("Euclid", "Manhattan", "Minkowski3"))
{
    draw_vote_series(diabetes, metric)
}

