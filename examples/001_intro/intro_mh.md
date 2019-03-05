### 2 main steps:
 | Purpose | command |
 | --- | --- |
 | **CAPTURE**            |`infer capture   -- <COMPILE/BUILD CMD>`|
 | **ANALYSIS**           |`infer analyze   -- <COMPILE/BUILD CMD>`|
 | **BOTH**               |`infer run       -- <COMPILE/BUILD CMD>`|

#### *How it does* (at least) as important as *what it does*

From an industrial point of view, the revolutionary side of Infer was 

      COMPOSITIONALITY/MODULARITY + INCREMENTAL ANALYSIS
Infer is analysing each function/method, following an order compatible with the call-graph to allow for intra-procedural analysis

The analysis of each function/method is blocked after an error is found, in the spirit of `incremental development`: fix the bug, then continue with the analysis.
As a side note, they tested how many bugs were fixed by developers with a *countinuous development tool* **VS** *batch tool* furnishing bugs the next morning, and found out 70% VS 0%.

<h4>INCREMENTAL ANALYSIS </h4>

Option | Purpose    
---|---
 `--reactive`  |             perform only the analysis of what changed in the code
  `--reactive --continue` |   continue with the previous execution of the incremental analysis (since bug fixing is interactive, as seen before)

#### INTEGRATION WITH SOME BUILD SYSTEM 

Build System | How to
--- | ---
CMake | `cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..`<p>`infer run --compilation-database build/compile_commands.json`
Make | `infer run -- make <target>` <p>Notice that Infer is substituting `gcc`, `g++`, `clang` etc.. with its own modified compilers, so the trick is not working if the path of the compiler is hardcoded in the make file!
 
 #### DIFFS
 ##### EXAMPLE WITH GIT, from the online guide

Suppose we have the branches `feature` and `master` and you want to compare them.
 ```bash
git checkout feature
git diff --name-only origin/feature..origin/master > index.txt
infer capture -- make -j 4 
infer analyze --changed-files-index index.txt
cp infer-out/report.json report-feature.json

git checkout master
infer capture --reactive -- make -j 4
infer analyze --reactive --changed-files-index index.txt
infer reportdiff --report-current report-feature.json --report-previous infer-out/report.json
 ```
 

#### KEEP IN MIND
**Infer** is **not** the most **accurate** tool; this is a price to pay to allow for fast analysis of millions of LOC (what about Google© which is at billions LOC?).
Personally, I would use it for SAFETY-CRITICAL applications only as a first approximative tool.
In that spirit, *bugs* are *heuristically filtered* by default, sometimes filtering out even very simple but ugly errors, as shown in `esempio001.c`.
You can disable the filtering with the option `--no-filtering`

### WHAT IS INFER CAPABLE OF!
Check your own tests, anyway you can find plenty of them in  

  https://github.com/facebook/infer/tree/master/infer/tests

Of course, you don't need to clone the whole rep! 7 MB of space are sufficient to save all the tests...

Browser add-on to download files/folders as ZIP from git : https://addons.mozilla.org/en-US/firefox/addon/gitzip/ 

If you have *svn*,
```bash
svn export https://github.com/facebook/infer/trunk/infer/tests
```
substituted `tree/master` with `trunk`

Configuration details can be specified in a file in the root folder of the project, `.inferconfig`

Infer option | purpose
--- | ---
`--debug` | debug mode, producing a bunch of interesting information for us
`--print-active-checkers` | print enabled checkers, as written in `infer-out/logs`
`-o <dir-name>` | specify a path for the results of the analysis, instead  of `project/infer-out`
`--[disable\|enable]-issue-type` | select the issues you want to trace
`--html` | produce an html with detail on the analysis process... look at it!

#### WHAT DID INFER DO? (A look at `infer-out`)

Infer is using an intermediate language, which is the same for C, C++, ObjC, Java.

In the folder `infer-out/specs` you can find the specifics of each function/method produced by the analysis. 

In theory we should be able to see the callgraph in folder `infer-out/captured` as described in the online manual. Actually to retrieve such information we have to *ask infer to print the `.dot` files*, e.g. directly by `--write-dotty` or implicitly by `--debug`, this way the `.dot` files will be in `infer-out/captured/<name>/` folder, in particular we have the control flow graph `icfg.dot` summentioned.

Command | Purpose
---|---
`infer report` | pretty print `.specs` file
  `--from-json-report` | how to use this @a-liut @lapotolo
`infer explore` | explore reports, useful when they are > 10
`--write-dotty` | write the `.dot` graphs in `infer-out/captured/<name>/`
  
