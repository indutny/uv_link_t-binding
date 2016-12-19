{
  "targets": [{
    "target_name": "binding",

    "dependencies": [
      "deps/uv_link_t/uv_link_t.gyp:uv_link_t",
    ],

    "include_dirs": [
      ".",
      "<!(node -e \"require('nan')\")",
    ],

    "sources": [
      "src/binding.cc",
      "src/observer.cc",
      "src/source.cc",
    ],
  }],
}
