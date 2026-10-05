package com.ememisya.llamacpp;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;

/**
 * Loads all native libraries required by llama.cpp from the classpath.
 *
 * <p>
 * All libraries are extracted into a single shared temporary directory so that
 * Windows, Linux, and macOS can resolve dependent libraries correctly.
 * </p>
 *
 * <p>
 * Only the JNI entrypoint libraries (llama_jni and tts_jni) are explicitly
 * loaded. All dependent libraries are resolved automatically by the operating
 * system's dynamic loader.
 * </p>
 */
public final class NativeLibraryLoader {

    /**
     * Indicates whether native libraries have already been loaded.
     */
    private static boolean loaded = false;

    /**
     * Shared temporary directory containing all extracted native libraries.
     */
    private static Path sharedTempDir;

    /**
     * Utility class constructor.
     *
     * <p>
     * Prevents instantiation.
     * </p>
     */
    private NativeLibraryLoader() {
    }

    /**
     * Loads all native libraries found on the application classpath.
     *
     * <p>
     * Libraries are first extracted into a shared temporary directory. JNI
     * entrypoint libraries are then loaded explicitly, allowing the operating
     * system to resolve any dependencies from the same location.
     * </p>
     *
     * @throws RuntimeException if native library extraction or loading fails
     */
    public static synchronized void loadAll() {
        if (loaded) {
            return;
        }

        try {
            List<String> libs = listNativeLibs();

            if (libs.isEmpty()) {
                throw new IllegalStateException(
                        "No native libraries found in classpath");
            }

            sharedTempDir = Files.createTempDirectory("jni_libs_");
            sharedTempDir.toFile().deleteOnExit();

            for (String lib : libs) {
                extractToSharedDir(lib);
            }

            for (String lib : libs) {
                if (lib.contains("llama_jni") || lib.contains("tts_jni")) {
                    System.load(sharedTempDir.resolve(lib).toString());
                }
            }

            if (isWindows()) {
                System.load(sharedTempDir.resolve("dllpath.dll").toString());
                setDllDirectory(sharedTempDir.toString());
            }

            loaded = true;

        } catch (Exception e) {
            throw new RuntimeException(
                    "Failed to load native libraries",
                    e);
        }
    }

    /**
     * Sets the Windows DLL search directory.
     *
     * <p>
     * Implemented by the supporting native dllpath library and only used on
     * Microsoft Windows.
     * </p>
     *
     * @param path path containing dependent DLLs
     */
    private static native void setDllDirectory(String path);

    /**
     * Determines whether the application is running on Microsoft Windows.
     *
     * @return true if the operating system is Windows, otherwise false
     */
    private static boolean isWindows() {
        return System.getProperty("os.name")
                .toLowerCase()
                .contains("win");
    }

    /**
     * Scans the classpath root for native library files.
     *
     * @return list of native library filenames
     * @throws Exception if the classpath cannot be scanned
     */
    private static List<String> listNativeLibs() throws Exception {
        URL root = NativeLibraryLoader.class.getResource("/");

        if (root == null) {
            return Collections.emptyList();
        }

        Path rootPath = Paths.get(root.toURI());

        try (var stream = Files.list(rootPath)) {
            return stream
                    .map(path -> path.getFileName().toString())
                    .filter(name
                            -> name.endsWith(".dll")
                    || name.endsWith(".so")
                    || name.endsWith(".dylib"))
                    .collect(Collectors.toList());
        }
    }

    /**
     * Extracts a native library from the classpath into the shared temporary
     * directory.
     *
     * @param filename native library filename
     * @throws IOException if the library cannot be found or copied
     */
    private static void extractToSharedDir(
            final String filename) throws IOException {

        try (InputStream in = NativeLibraryLoader.class
                .getResourceAsStream("/" + filename)) {

            if (in == null) {
                throw new FileNotFoundException(
                        "Native library not found: " + filename);
            }

            Path out = sharedTempDir.resolve(filename);

            Files.copy(
                    in,
                    out,
                    StandardCopyOption.REPLACE_EXISTING);

            out.toFile().deleteOnExit();
        }
    }
}
