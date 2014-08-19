/**
 * @file Gruntfile for the minification of JavaScript and compilation of Bootstrap
 * @author griffinj@lafayette
 *
 */
module.exports = function(grunt) {

    // Project configuration.
    grunt.initConfig({
	    pkg: grunt.file.readJSON('package.json'),
	    uglify: {
		options: {
		    banner: '/**\n' + ' * @file <%= pkg.name %> Theme by the Lafayette College Libraries last built on <%= grunt.template.today("yyyy-mm-dd") %>\n' + ' * @see {@link https://github.com/LafayetteCollegeLibraries Lafayette College Libraries}\n' + ' */\n'
		},
		dynamic_mappings: {
		    files: [
			    {
			        expand: true,
			        cwd: 'src/',
			        src: ['**/*.js'],
			        dest: 'build/',
			        ext: '.min.js',
			        extDot: 'first'
			    },
			    ],
		},
	    },
	    bootstrap: {
		dest: 'build',
		js: [
		     //'bootstrap-modal.js'
		     ],
		css: [
		      'less/overrides.less'
		      ]
	    }
	});

    // Load the plugin that provides the "uglify" task.
    grunt.loadNpmTasks('grunt-contrib-uglify');

    // For building Bootstrap
    grunt.loadNpmTasks('grunt-bootstrap');

    // Default task(s).
    grunt.registerTask('default', ['uglify']);
};
