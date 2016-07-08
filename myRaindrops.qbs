import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "../../../libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: "../../.."

    ofApp {
        name: { return FileInfo.baseName(path) }

        files: [
            'src/Background.h',
            'src/ColorSchemes.h',
            'src/Drop.h',
            'src/LevelLoader.h',
            'src/Plant.cpp',
            'src/Plant.h',
            'src/PolygonMan.cpp',
            'src/PolygonMan.h',
            'src/RaindropLine.h',
            'src/WaterParticles.h',
            'src/contactListener.h',
            'src/costumSoftBody.h',
            'src/gameScene.cpp',
            'src/gameScene.h',
            'src/linkedList.cpp',
            'src/linkedList.h',
            'src/main.cpp',
            'src/ofApp.cpp',
            'src/ofApp.h',
            'src/rapidxml/license.txt',
            'src/rapidxml/manual.html',
            'src/rapidxml/rapidxml.hpp',
            'src/rapidxml/rapidxml_iterators.hpp',
            'src/rapidxml/rapidxml_print.hpp',
            'src/rapidxml/rapidxml_utils.hpp',
            'src/testApp.cpp',
            'src/testApp.h',
        ]

        of.addons: [
            'ofxBox2d',
            'ofxContourAnalysis',
            'ofxOpenCv',
            'ofxVectorMath',
            'ofxTriangle',
            'ofxNoise',
        ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: []       // list of additional system pkgs to include
        of.includePaths: []     // include search paths
        of.cFlags: []           // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: []      // flags passed to the linker
        of.defines: []          // defines are passed as -D to the compiler
                                // and can be checked with #ifdef or #if in the code

        // other flags can be set through the cpp module: http://doc.qt.io/qbs/cpp-module.html
        // eg: this will enable ccache when compiling
        //
        // cpp.compilerWrapper: 'ccache'

        Depends{
            name: "cpp"
        }

        // common rules that parse the include search paths, core libraries...
        Depends{
            name: "of"
        }

        // dependency with the OF library
        Depends{
            name: "openFrameworks"
        }
    }

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}
