#!bin/bash

echo "Building GnomeKeyBinder..."

#build project and it's tests 
mkdir build
cd build    
cmake ..
make -j$(nproc)

#run tests
echo Running tests...
./GnomeKeyBinderTest --log_level=test_suite || true

cd ..




###TODO: do this last

# # Setup bash completion
# echo -e "\nSetting up command completion..."
# cat > gnomekeybinder-completion.bash << 'EOF'
# _gnomekeybinder_complete() {
#     local cur prev opts
#     COMPREPLY=()
#     cur="${COMP_WORDS[COMP_CWORD]}"
#     prev="${COMP_WORDS[COMP_CWORD-1]}"
#     opts="--help --view-schema --reset-schema --add-key --remove-key"

#     if [[ ${cur} == -* ]] ; then
#         COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
#         return 0
#     fi
# }
# complete -F _gnomekeybinder_complete GnomeKeyBinder
# complete -F _gnomekeybinder_complete ./GnomeKeyBinder
# complete -F _gnomekeybinder_complete ./build/GnomeKeyBinder
# EOF

# # Install completion script
# mkdir -p ~/.local/share/bash-completion/completions/
# cp gnomekeybinder-completion.bash ~/.local/share/bash-completion/completions/

# echo -e "\nTo enable autocompletion now, run:"
# echo "source ~/.local/share/bash-completion/completions/gnomekeybinder-completion.bash"
# echo -e "\nBuild complete! Run with:"
# echo "./build/GnomeKeyBinder [options]"