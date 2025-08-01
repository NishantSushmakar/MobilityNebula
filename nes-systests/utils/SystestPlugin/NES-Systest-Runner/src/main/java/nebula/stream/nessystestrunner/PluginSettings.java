/*
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        https://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

package nebula.stream.nessystestrunner;

import com.intellij.openapi.components.PersistentStateComponent;
import com.intellij.openapi.components.ServiceManager;
import com.intellij.openapi.components.State;
import com.intellij.openapi.components.Storage;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

@State(
        name = "com.example.nessystestrunner.PluginSettings",
        storages = @Storage("PluginSettings.xml")
)

/// Plugin Settings of the NES-Systest-Runner Plugin displayed in Settings/Tools/NES-Systest-Runner
/// NOTE: All settings have become obsolete for now and have been hidden
public class PluginSettings implements PersistentStateComponent<PluginSettings> {
    private String pathSetting = "";
    private boolean dockerCommandCheckBox;
    private String dockerCommand = "";
    private String dockerTestFilePath = "";

    public static PluginSettings getInstance() {
        return ServiceManager.getService(PluginSettings.class);
    }

    @Nullable
    @Override
    public PluginSettings getState() {
        return this;
    }

    @Override
    public void loadState(@NotNull PluginSettings state) {
        this.pathSetting = state.pathSetting;
        this.dockerCommand = state.dockerCommand;
        this.dockerCommandCheckBox = state.dockerCommandCheckBox;
        this.dockerTestFilePath = state.dockerTestFilePath;
    }

    public String getPathSetting() {
        return pathSetting;
    }

    public void setPathSetting(String pathSetting) {
        this.pathSetting = pathSetting;
    }

    public String getDockerCommand() {
        return dockerCommand;
    }

    public void setDockerCommand(String dockerCommand) {
        this.dockerCommand = dockerCommand;
    }

    public boolean getDockerCommandCheckBox() {
        return dockerCommandCheckBox;
    }

    public void setDockerCommandCheckBox(boolean dockerCommandCheckBox) {
        this.dockerCommandCheckBox = dockerCommandCheckBox;
    }

    public String getDockerTestFilePath() {
        return dockerTestFilePath;
    }

    public void setDockerTestFilePath(String dockerTestFilePath) {
        this.dockerTestFilePath = dockerTestFilePath;
    }
}
