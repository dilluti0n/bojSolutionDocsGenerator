compile:
	gcc -o bojSolGen -UBOJDIRPATH src/*.c
run:
	./bojSolGen
pcompile:
	gcc -o bojSolGen -UBOJDIRPATH src/*.c
personal:
	./bojSolGen
	cd /home/hskim/my-site/baekjun
	git add .
	git commit -m 'auto commit by BSDG'
	git push -u origin main