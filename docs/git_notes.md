## to remove from git only all files ending with:

git rm --cached '*.o' 


## to remove folder from git 
 git rm -r --cached <folder-name>



## remove large file from history
git filter-branch --tree-filter 'rm -f /blender/models/first_respectable_terrain.blend' HEAD