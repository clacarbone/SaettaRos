FILE(REMOVE_RECURSE
  "msg_gen"
  "src/Saetta_Base/msg"
  "msg_gen"
  "CMakeFiles/rospack_gencfg"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/rospack_gencfg.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
