{
  "targets": [{
    "target_name": "uv_link_t",
    "type": "static_library",

    "direct_dependent_settings": {
      "include_dirs": [ "include" ],
    },

    "include_dirs": [
      ".",
    ],

    "sources": [
      "src/defaults.c",
      "src/uv_link_t.c",
      "src/uv_link_observer_t.c",
      "src/uv_link_source_t.c",
    ],
  }],
}
