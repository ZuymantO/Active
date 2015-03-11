
#Les regles de construction pour chaque executable
all:
	mkdir -p ./bin
	mkdir -p ./obj
#	cd src && make all && cd ..
	cd src && make all_obj && cd ..

#Mettre ici toutes les regles de clean pour chaque module
clean:
	cd src && make clean_all && cd ..
	rm *~ ./src/*~ ./src/daemon/*~ ./src/bi/*~
	rm -rf ./bin/*
	rm -rf ./obj/*

