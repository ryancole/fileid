{
    "targets": [
        {
            "target_name": "fileid",
            "sources": ["src/fileid.cc"],
            "include_dirs": ["/usr/local/include/fileid"],
            "link_settings": {
                "libraries": ["-lsc_fi"],
                "ldflags": ["-L/usr/local/lib/fileid"]
            }
        }
    ]
}