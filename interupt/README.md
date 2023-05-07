包含引导程序和相关文件，暂时支持 BIOS 启动，不支持 UEFI 的方式

启动过程：
    bios 加载 boot 至 mbr,然后 boot 加载 loader, loader 配置好后，再进入 kernel
