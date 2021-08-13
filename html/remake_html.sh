#!/bin/sh

for categ in MTR VTR
do

    for year in 2017 2018
    do

	cp -p index_topdir.html index_${categ}_${year}.html
	
	sed -i "s/YEAR/${year}/g" index_${categ}_${year}.html
	sed -i "s/CAT/${categ}/g" index_${categ}_${year}.html
	
	for region in A B CR SR HFNoiseCR HFNoise
	do
	    cp -p index_template_${categ}.html index_${region}_${categ}_${year}.html

	    sed -i "s/YEAR/${year}/g" index_${region}_${categ}_${year}.html
	    sed -i "s/CAT/${categ}/g" index_${region}_${categ}_${year}.html
	    sed -i "s/REGION/${region}/g" index_${region}_${categ}_${year}.html

	done
    done
done
