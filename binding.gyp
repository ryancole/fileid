{
    "targets": [
        {
            "target_name": "ofi",
            "sources": ["src/ofi.cc"],
            "include_dirs": ["/usr/local/include"],
            "link_settings": {
                "libraries": ["-lsc_fi"],
                "ldflags": ["-L/usr/local/lib"]
            }
        }
    ]
}