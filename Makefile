path={your_path}
x = l
dpath=/home/hskim/my-site/baekjun
lpath=/Users/hskimse/Desktop/projects/baekjun
dep=-lpthread -lcurl -ltidy

compile:
	gcc -o bojSolGen -D BOJDIRPATH="\".\"" src/*.c $(dep)
ucompile:
	gcc -o bojSolGen -D BOJDIRPATH="\"$(path)\"" src/*.c $(dep)
jcompile:
	gcc -o bojSolGen src/*.c $(dep)
run:
	./bojSolGen
	git -C $(path) add .
	git -C $(path) commit -m 'auto commit by BSDG'
	git -C $(path) push -u origin main
	git -C $(path) status
$xcompile:
	gcc -o bojSolGen -D BOJDIRPATH="\"$($xpath)\"" src/*.c $(dep)
$xrun:
	./bojSolGen
	git -C $($xpath) add .
	git -C $($xpath) commit -m 'auto commit by BSDG'
	git -C $($xpath) push -u origin main
	git -C $($xpath) status


