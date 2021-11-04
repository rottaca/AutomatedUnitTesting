# CMakeList.txt: CMake-Projekt für "AutomatedUnitTesting". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

# Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
add_executable (tests "test.cpp" )

target_link_libraries(tests PRIVATE AutomatedUnitTesting)

# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.
