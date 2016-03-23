Rules
------------------------------------------------------------

1. Any new library requires corresponding test class (QTest) which
must pass and have reasonable coverage.

2. A PR should, if possible, be merged by someone other than
the author.

3. A PR can only be merged into the development branch if
all tests in all libraries pass, and the application compiles
and runs (i.e. development branch should never be broken).

4. Do your very best to make each commit a unified and coherent
set of changes which still allows the given library/subapplication
to compile. If you have to push a commit to github which is partial work,
e.g. due to end of day migration to another workstation, then make 
a temporary WIP commit which you remove when you continue working
the next time (see below). This should generally only be
done when you are working alone on a branch

Setup
------------------------------------------------------------

1. Fork main repo on github, herein referred to as upstream,
to your own github account, resulting in a new repo herein
referred to as origin.

2. Clone the origin repo locally to your 
  
    https: `git clone https://github.com/username/JoyStream.git` 
  
    ssh: `git clone git@github.com:username/JoyStream.git`

3. Setup upstream remote.
  
    `cd JoyStream`
  
    https: `git remote add upstream https://github.com/JoyStreamApp/JoyStream.git`
  
    ssh: `git remote add upstream git@github.com:JoyStreamApp/JoyStream.git`

4. Create local development branch.
  
    `git checkout -b development --track origin/development`

Workflow
------------------------------------------------------------

All work on new features is done in branche from the development branch,
and go into the upstream/development through a pull request (PR) on github.

1. Check out the development branch

    `git checkout development`

2. Make sure to have latest commits in upstream/development
  
    `git pull upstream development`

3. Create new, appropriately named (e.g. BRANCH_NAME), branch of the development branch
  
    `git checkout -b BRANCH_NAME`

4. Push new branch to your origin, such that a PR can be opened between it and upstream

    `git push origin BRANCH_NAME`

5. Open up PR on github, with name of the form WIP: *,
between the origin/BRANCH_NAME and the upstream/development.

If the work has multiple subgoals, then add a checklist
which you manage as you complete/add goals.

6. If you have to push an incomplete commit to github, name it with WIP prefix. To continue
working from such a commit, first remove it locally while leaving the workspace intact

    `git reset HEAD^1`

and then push this removal to origin/BRANCH_NAME

    `git push origin <BRANCH_NAME> --force`

7. Try to push local commits to origin/BRANCH_NAME as frequently as possible,
so that others can keep track of your work through the PR on github.
 
    `git push origin BRANCH_NAME`

8. If you require new work which is merged into upstream/development, then
rebase your branch on top of the most recent changes upstream.

    `git pull --rebase upstream development`

NOTE: We rebase, rather than merge, to avoid introducing arbitrary
merge commits in your own branch.

9. Eventually, you will have pushed your last work commit to origin/BRANCH_NAME.
If there are no merge conflicts between it and upstream/development, you are done.

If there are merge conflicts, you will have to resolve them in
one of two ways.

a) Pull in and merge upstream/development into BRANCH_NAME locally, resolving
any possibly conflicts into a merge commit.

    `git pull upstream development`

b) Interactively rebase BRANCH_NAME locally on top of upstream/development.
  
    `git pull --rebase upstream development`

10. Push new commits to origin/BRANCH_NAME
 
    `git push origin BRANCH_NAME`
