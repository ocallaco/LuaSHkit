package = "redis-async"
version = "scm-0"

source = {
   url = "git://github.com/ocallaco/LuaSHkit",
   branch = "master",
   dir = "LuaSHkit",
}

description = {
   summary = "A lua wrapper for lshkit",
   detailed = [[
A lua wrapper for lshkit -- Wei Dong, http://lshkit.sourceforge.net/
   ]],
   homepage = "https://github.com/ocallaco/LuaSHkit",
   license = "BSD"
}

dependencies = {
   "sys >= 1.0",
}

build = {
   type = "builtin",
   modules = {
      ['luashkit.init'] = 'init.lua',
   }
}
