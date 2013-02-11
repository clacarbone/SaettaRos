FILE(REMOVE_RECURSE
  "msg_gen"
  "src/Saetta_Base/msg"
  "msg_gen"
  "CMakeFiles/ROSBUILD_gencfg_cpp"
  "cfg/cpp/Saetta_Base/Saetta_Base_paramsConfig.h"
  "docs/Saetta_Base_paramsConfig.dox"
  "docs/Saetta_Base_paramsConfig-usage.dox"
  "src/Saetta_Base/cfg/Saetta_Base_paramsConfig.py"
  "docs/Saetta_Base_paramsConfig.wikidoc"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_gencfg_cpp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
