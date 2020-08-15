import qbs
import qbs.FileInfo

StaticLibrary {
    name: "scmrtos"

    Depends { name: 'cpp' }
    Depends { name: 'common_options' }
    Depends { name: 'stm8lib' }

    property string platformPath:
        qbs.architecture.contains("stm8") ? "port/stm8/iar/" : "port/cortex/mx-gcc/"

    property string configPath: FileInfo.cleanPath(FileInfo.joinPaths(sourceDirectory, "../config"))

    cpp.includePaths: [
        FileInfo.joinPaths(sourceDirectory, "core"),
        FileInfo.joinPaths(sourceDirectory, platformPath),
        configPath
    ]

    cpp.assemblerFlags: [
        "-I" + configPath
    ]

    Group {
        name: "Config"
        prefix: configPath + "/"
        files: [
            "*.h"
        ]
    }

    Group {
        name: "Core"
        prefix: "core/"
        files: [
            "*.h",
            "*.cpp"
        ]
    }

    Group {
        name: "Platform"
        prefix: platformPath
        files: [
            "*.h",
            "*.cpp",
            "*.s"
        ]
    }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [
            FileInfo.joinPaths(product.sourceDirectory, "core"),
            FileInfo.joinPaths(product.sourceDirectory, product.platformPath),
            product.configPath
        ]
    }
}
