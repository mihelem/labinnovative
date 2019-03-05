## DIFF: GIT+INFER

Open an empty folder, which will be the root of our project.
Write the files `bugger.c`, `main.c` and a makefile as follows:

```C
/* bugger.c :: master branch */
int *creator(){
    int *A = (int *)malloc(sizeof(int)*100);
    return A;
}
```

```C
/* main.c :: master branch */
#include <stdio.h>
#include <stdlib.h>

#include "bugger.c"

int reader( int i ){
    int result = 1;
    while( i>0 ){
        result *= creator()[i];
        i >>= 1;
    }
    return result;
}

int main(int argc, char *argv[]){
    int j = argc>1 ? atoi(argv[1]) : 50;
    printf("%d\n", read(j));
}
```
(do you see the bugs? :)
then init/stage/commit changes and create/checkout a new branch:
```bash
git init .
git add .
git commit
git checkout -b fix1
```

then partially fix the bugs in `main.c`
```C
/* main.c :: fix1 branch */
#include <stdio.h>
#include <stdlib.h>

#include "bugger.c"

int reader( int i ){
    int result = 1;
    int *tmp_ptr = NULL;
    while( i>0 ){
        if( tmp_ptr = creator() ){
	        result *= tmp_ptr[i];
        	free(tmp_ptr);
		    }
		i >>= 1;
    }
    return result;
}

int main(int argc, char *argv[]){
    int j = argc>1 ? atoi(argv[1]) : 50;
    printf("%d\n", read(j));
}
```

Again stage/commit changes, then save the index of different files between the two branches in a file, e.g. `index.txt`:
```bash
git add .
git commit
git diff --name-only fix1..master > index.txt
```
Continue following the procedure described in the [introduction](./intro_mh.md), with a twist in the details to check that the report diff is not considering which checker is enabled:
```bash
infer capture -- make 
infer analyze --no-filtering --bufferoverrun --changed-files-index index.txt
cp infer-out/report.json report-fix1.json

git checkout master
infer capture --reactive -- make
infer analyze --reactive --no-filtering --changed-files-index index.txt
infer reportdiff --report-current report-fix1.json --report-previous infer-out/report.json
```

Notice that I analyzed the `fix1` branch with the experimental `--bufferoverrun` checker, which, by symbolic transformation of ranges, will find out that there is a buffer overrun; the `master` branch instead is not analyzed with such a checker.
At this point, `infer-reportdiff` will have written 3 `.json` reports in `infer-out/differential`; you can easily check that it appears as if we had fixed 2 bugs (memory leak and null dereference), while adding a new one. Of course the *new one* would be included in the `preexisting.json` report if we had analyzed with the same checkers both branches!


