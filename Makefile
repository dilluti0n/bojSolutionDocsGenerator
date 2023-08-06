path={your_path}
x = d
dpath=/home/hskim/my-site/baekjun
lpath=/Users/hskimse/Desktop/백준

compile:
	gcc -o bojSolGen -D BOJDIRPATH="\"./\"" src/*.c
ucompile:
	gcc -o bojSolGen -D BOJDIRPATH="\"$(path)\"" src/*.c
jcompile:
	gcc -o bojSolGen src/*.c
run:
	./bojSolGen
	git -C $(path) add .
	git -C $(path) commit -m 'auto commit by BSDG'
	git -C $(path) push -u origin main
	git -C $(path) status
$xcompile:
	gcc -o bojSolGen -D BOJDIRPATH="\"$($xpath)\"" src/*.c
$xrun:
	./bojSolGen
	git -C $($xpath) add .
	git -C $($xpath) commit -m 'auto commit by BSDG'
	git -C $($xpath) push -u origin main
	git -C $($xpath) status

done