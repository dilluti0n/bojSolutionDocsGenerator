path={your_path}
x = d
dpath=/home/hskim/my-site/baekjun
lpath=/Users/hskimse/Desktop/projects/baekjun
dep=-lcurl -ltidy

compile:
	gcc -o bojSolGen -D BOJDIRPATH="\".\"" -D __GITCOMMENDLINEMACRO__="1" src/*.c $(dep)
ucompile:
	gcc -o bojSolGen -D BOJDIRPATH="\"$(path)\"" src/*.c $(dep)
jcompile:
	gcc -o bojSolGen src/*.c $(dep)
run:
	./bojSolGen

$xcompile:
	gcc -o bojSolGen -D BOJDIRPATH="\"$($xpath)\"" src/*.c $(dep)


