KERNEL_NAME=$(uname -s)
if [[ ${KERNEL_NAME} =~ ^MSYS_NT-.* ]]; then
    PLATFORM=msys2
elif [[ ${KERNEL_NAME} =~ Linux ]]; then
    PLATFORM=linux
else
    echo "Unknown platform" >&2
    exit 1
fi
echo ${PLATFORM}
exit 0