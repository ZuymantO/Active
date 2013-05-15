
#Les regles de construction pour chaque executable
all:
	cd tinyxml_test && make all
	cd daemon && make all && cd ..
	cd bdd && make all && cd ..
	cd ui && make all && cd ..
	cd ../
#Mettre ici toutes les regles de clean pour chaque module
clean:
	rm -rf ./*.o ./*~
	cd tinyxml_test && make clean
	cd daemon && make clean
