# XoneKey GitHub Actions Guide

This guide explains how to use the automated build and release system configured for XoneKey.

## 🏗️ Continuous Integration (CI)

Every time you push code to the `master` branch or create a Pull Request, the **MSBuild** workflow runs automatically.

-   **Goal**: Verifies that the Windows project still compiles for both `x86` and `x64` architectures.
-   **Artifacts**: You can find compiled binaries in the "Actions" tab by clicking on a specific run.

## 📦 Automated Releases

I have configured the workflow to simplify the release process. When you are ready to publish a new version:

1.  **Update Version**: Ensure `version.json` and `CHANGELOG.md` are updated.
2.  **Push a Tag**: Push a tag starting with `v` (e.g., `v1.0.4`).
    ```bash
    git tag v1.0.4
    git push origin v1.0.4
    ```
3.  **Automatic Build**: GitHub Actions will:
    -   Build both `x86` and `x64` versions.
    -   Package the `.exe` files into a single `XoneKey-v1.0.4.zip`.
    -   **Create a Draft Release**: A new draft will appear on your [Releases](https://github.com/xonevn-ai/xonekey/releases) page.
4.  **Publish**:
    -   Go to the draft release.
    -   Check the generated release notes.
    -   Click **Publish Release** to make it public.

## 🛠️ Configuration Details

The workflow is located at `.github/workflows/msbuild.yml`.

-   **Environment**: Uses `windows-latest` runners.
-   **Packaging**: Uses PowerShell to structure the `dist` folder before zipping.
-   **Release**: Uses `softprops/action-gh-release` for the actual GitHub Release creation.
