Rules
------------------------------------------------------------

1. Any new library requires corresponding QTest class which
must pass and have reasonable coverage.

2. A PR should, if possible, be approved by someone other than
the author.

3. A PR can only be merged into the development branch if
all tests in all libraries pass, and the application compiles
and runs (i.e. development branch should never be broken).

4. Do your very best to make each commit a unified and coherent
set of changes which still allows the given library/subapplication
to compile. If you have to push commits which are partial work
due to end of day migration to another workstation, then make 
a temporary WIP commit which you remove/squash when you continue working.

Initial setup
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

Development workflow
------------------------------------------------------------

1. Create new feature branch based off upstream development branch.
  
    Make sure to have latest commits in your local dev branch
  
    `git checkout development`
  
    `git pull upstream development`
  
    `git checkout -b <NAME OF NEW BRANCH>` -> (creates the new branch)

2. Open up PR on github with name of the form WIP <DESCRIPTION>, and 
if work will include multiple subgoals, then add a checklist which you manage
as you complete/add goals.

3. When you are ready to push your new feature to your origin repo, do a rebase in case the upstream dev branch
has changed since you started working on the branch. 
  
    `git pull --rebase upstream development` -> (resolve any merge conflicts)
  
    `git push origin <NAME OF NEW BRANCH>`

4. Create a pull request targeting the upstream development branch.
    At this point you can goto github and create a pull request origin:new_feature ---> upstream:development
  
    You can still update the branch (step 2) and push more commits which will show up in the PR (Pull request).
  
    When the PR is merged, you can delete your local new_feature branch, and pull down from latest development (which should now have your new commits)







