------------------------------------------------------------
------------------------------------------------------------
<h1>GitHub Workflow</h1>
------------------------------------------------------------
------------------------------------------------------------


To use ssh-key for login, see 
- https://help.github.com/articles/generating-ssh-keys/
- https://gist.github.com/romuloceccon/2756ea5c8fbee65d73d0
- https://help.github.com/articles/what-are-github-s-ssh-key-fingerprints/

------------------------------------------------------------
Initial setup
------------------------------------------------------------

1. Fork main repo on github.

2. Clone the (origin) forked repo locally
  
    https: `git clone https://github.com/username/JoyStream.git` 
  
    ssh: `git clone git@github.com:username/JoyStream.git`

3. Setup upstream remote.
  
    `cd JoyStream`
  
    https: `git remote add upstream https://github.com/JoyStreamApp/JoyStream.git`
  
    ssh: `git remote add upstream git@github.com:JoyStreamApp/JoyStream.git`

4. Create local development branch.
  
    `git checkout -b development --track origin/development`

------------------------------------------------------------
Development workflow
------------------------------------------------------------
1. Create new feature branch based off upstream development branch.
  
    Make sure to have latest commits in your local dev branch
  
    `git checkout development`
  
    `git pull upstream development` -> (same as fetch, merge)
  
    `git checkout -b new_feature` -> (creates the new branch)

    Add new code...

    `git commit -m "msg"`

2. You are now ready to push your new feature to your origin repo. In case the upstream dev branch
has changed since you started working on the branch, lets rebase: 
  
    `git pull --rebase upstream development` -> (resolve any merge conflicts)
  
    `git push origin new_feature`

3. Create a pull request targeting the upstream development branch.
    At this point you can goto github and create a pull request origin:new_feature ---> upstream:development
  
    You can still update the branch (step 2) and push more commits which will show up in the PR (Pull request).
  
    When the PR is merged, you can delete your local new_feature branch, and pull down from latest development (which should now have your new commits)


-------------------------------------------------------------------------------
<h4>Useful links about merging and rebasing</h4>
-------------------------------------------------------------------------------
- https://www.atlassian.com/git/articles/git-team-workflows-merge-or-rebase/

- https://git-scm.com/book/en/v2/Git-Branching-Rebasing

- https://kernel.org/pub/software/scm/git/docs/git-rebase.html







