require "bundler/gem_tasks"
require 'rake/extensiontask'

Dir.glob('tasks/**/*.rake').each(&method(:import))

Rake::ExtensionTask.new('fast_camelize')

task :default => :spec
