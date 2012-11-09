{
    "targets": [
        {
            "target_name": "fileid",
            "sources": ["src/fileid.cc"],
            "include_dirs": ["/usr/local/include"],
            "link_settings": {
                "libraries": ["-lsc_fi"],
                "ldflags": ["-L/usr/local/lib"]
            }
        }
    ]
}