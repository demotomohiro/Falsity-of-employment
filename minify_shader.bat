@mkdir preprocesseds
call ..\lib4kb\bin\minify-preserve-externals.bat particle_render.vs preprocesseds
call ..\lib4kb\bin\minify-preserve-externals.bat particle_render.fs preprocesseds
call ..\lib4kb\bin\minify-preserve-externals.bat particle_dynamics.vs preprocesseds
call ..\lib4kb\bin\minify-preserve-externals.bat sound.vs preprocesseds

