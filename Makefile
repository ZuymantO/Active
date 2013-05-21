
#Les regles de construction pour chaque executable
all:
	mkdir -p ./bin
	mkdir -p ./obj
	cd src && make all && cd ..

#Mettre ici toutes les regles de clean pour chaque module
clean:
	cd src && make clean && cd ..
	rm -rf ./bin/*
	rm -rf ./obj/*

