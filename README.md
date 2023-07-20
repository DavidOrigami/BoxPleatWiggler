BoxPleatWiggler

This is inspired by [Brandon Wong's](https://web.mit.edu/wongb/www/origami/index.html) [Wiggler](https://web.mit.edu/wongb/www/origami/resources/wiggle.html)

Things like Oripa itself and excessive moving and copypasting in Orihime/Oriedita make vertices inaccurate. 
This little program fixes the ones that lie on any grid. It can't fix vertices that use 22.5 angle references. 

Instructions:

1. Drag and drop your broken .cp file onto the executable
2. Enter the grid size. This number is  not necessarily the grid size of the model!
   This number should be the grid size of a grid that is so dense, that every vertex that was made with standard boxpleating lies on a grid point. 
   In other words set it the the projects grid size (or multiples if you have partials)

   Examples:
     - If you have a 32 grid model that doesn't have any partials and it's either from Oripa or you correctly set the Grid size to 32 in Orihime/Oriedita, enter 32
     - If that model has partials from 32 to 64 and it's either from Oripa or you correctly set the Grid size to 32 in Orihime/Oriedita, enter 64
     - If you don't have the projects grid size set to the models actual grid size, use the grid size of the project (or multiples to accommodate for partials)
     - If you have multiple varying grid sizes and both need to be fixed, use a grid size that accommodates all (eg. for 32 and 48 grid use 96) or just run BoxPleatWiggler multiple times
