# -*- python -*-

def initial_scons_config():
    SConsignFile('build/sconsign.dblite')
    Decider('MD5-timestamp')

def add_special_methods(env):
    boost_libs = ['boost_unit_test_framework-mt']
    boost_defines = ['BOOST_TEST_DYN_LINK']

    def run_unit_test(env, target, source):
        import subprocess
        app = str(source[0].abspath)
        if not subprocess.call(app):
            open(str(target[0]),'w').write("PASSED\n")
        else:
            return 1

    def build_and_run_test(env, src, lib, depLibs = []):
        lib_name = str(lib[0]).replace('.', '_')
        test = env.Program(lib_name + "_unit_tests",
                           src,
                           LIBS=[lib] + depLibs + boost_libs,
                           CPPDEFINES=boost_defines)
        env.Command(lib_name + "_test_passed.txt", test, run_unit_test)

    env.AddMethod(build_and_run_test, "BoostTests")

def create_env():
    env = Environment(CC = 'gcc-6', CXX = 'g++-6')
    Export('env')
    env.Append(CCFLAGS=['-Wall', '-Wextra', '-Wpedantic', '-Werror',
                        '--std=c++14', '-O2'])
    return env

def build_executable(env):
    p = SConscript('src/SConscript', variant_dir='build/', duplicate=0)
    env.Install('./', p)


initial_scons_config()
env = create_env()
add_special_methods(env)
build_executable(env)
