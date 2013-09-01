def options(opt):
    opt.load('compiler_c')

def configure(conf):
    conf.load('compiler_c')
    conf.env.append_unique('CFLAGS', ['-std=c99', '-Wall'])
    try:
        conf.check_cc(header_name='iconv.h', function_name='iconv',
                uselib_store='ICONV', msg='Checking for default iconv')
    except:
        conf.check_cc(lib='iconv', header_name='iconv.h', function_name='iconv',
                uselib_store='ICONV', msg='Checking for library iconv')

def build(bld):
    bld.program(source='hr.c', target='hr', defines='_XOPEN_SOURCE',
            lib=bld.env['LIB_ICONV'])
    bld.install_files('${PREFIX}/bin', ['hr'])
