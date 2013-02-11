FILE(REMOVE_RECURSE
  "msg_gen"
  "src/Saetta_Base/msg"
  "msg_gen"
  "CMakeFiles/ROSBUILD_genmsg_lisp"
  "msg_gen/lisp/Saetta_Base_data.lisp"
  "msg_gen/lisp/_package.lisp"
  "msg_gen/lisp/_package_Saetta_Base_data.lisp"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_lisp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
