require 'bundler/gem_tasks'
require 'rake/extensiontask'

Dir.glob('tasks/**/*.rake').each(&method(:import))

Rake::ExtensionTask.new('fast_camelize') do |extension|
  extension.lib_dir = 'lib/fast_camelize'
end

task :default => :spec
