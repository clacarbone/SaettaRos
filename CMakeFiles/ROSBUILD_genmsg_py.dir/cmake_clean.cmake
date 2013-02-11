FILE(REMOVE_RECURSE
  "msg_gen"
  "src/Saetta_Base/msg"
  "msg_gen"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "src/Saetta_Base/msg/__init__.py"
  "src/Saetta_Base/msg/_Saetta_Base_data.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
