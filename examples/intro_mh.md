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
In theory we should be able to see the callgraph in folder `infer-out/captured` as described in the online manual, but the folder is not created, maybe the sql file substituted the .cfg, .cg, .tenv files?

The analysis of each function/method is blocked after an error is found, in the spirit of `incremental development`: fix the bug, then continue with the analysis.
As a side note, they tested how many bugs were fixed by developers with a *countinuous development tool* **VS** *batch tool* furnishing bugs the next morning, and found out 70% VS 0%.

KEEP IN MIND: **Infer** is **not** the most **accurate** tool; this is a price to pay to allow for fast analysis of millions of LOC
Personally, I would use it for SAFETY-CRITICAL applications only as a first approximative tool.
In that spirit, *bugs* are *heuristically filtered* by default, sometimes filtering out even very simple but ugly errors, as shown in `esempio001.c`.
You can disable the filtering with the option `--no-filtering`

<h4>INCREMENTAL ANALYSIS </h4>

Option | Purpose    
---|---
 `--reactive`  |             perform only the analysis of what changed in the code
  `--reactive --continue` |   continue with the previous execution of the incremental analysis (since bug fixing is interactive, as seen before)


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

#### A look at `infer-out`

Infer is using an intermediate language, which is the same for C, C++, ObjC, Java.

In the folder `infer-out/specs` you can find the specifics of each function/method produced by the analysis. 

Command | Purpose
---|---
`infer report` | pretty print `.specs` file
  `--from-json-report` | option to read from the json reports (todo)
  
