// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <frc/MathUtil.h>
#include <frc2/command/button/Trigger.h>

#include "commands/Autos.h"
#include "commands/ExampleCommand.h"

RobotContainer::RobotContainer() {
  // Initialize all of your commands and subsystems here
  // m_drive = Drivetrain();  // don't initialize this here as the constructor is called again and duplicates assignment of hardware to pins
  // Configure the button bindings
  ConfigureBindings();
}

void RobotContainer::ConfigureBindings() {
  m_drive.SetDefaultCommand(GetArcadeDriveCommand().ToPtr());

  // Configure your trigger bindings here

  // Schedule `ExampleCommand` when `exampleCondition` changes to `true`
  // frc2::Trigger([this] {
  //   return m_subsystem.ExampleCondition();
  // }).OnTrue(ExampleCommand(&m_subsystem).ToPtr());

  // Schedule `ExampleMethodCommand` when the Xbox controller's B button is
  // pressed, cancelling on release.
  // m_driverController.B().WhileTrue(m_subsystem.ExampleMethodCommand());


}

frc2::CommandPtr RobotContainer::GetAutonomousCommand() {
  // An example command will be run in autonomous
  return autos::ExampleAuto(&m_subsystem);
}

frc2::RunCommand RobotContainer::GetArcadeDriveCommand() {
  return frc2::RunCommand(
    [this] {
      m_drive.ArcadeDrive(
        frc::ApplyDeadband<double>(-m_driverController.GetLeftY(), kDeadBand),
        frc::ApplyDeadband<double>(m_driverController.GetRightX(), kDeadBand));
    },
    {&m_drive}
  );
}

frc2::FunctionalCommand RobotContainer::GetDriveUntilDistCommand(units::meter_t dist) {
  return frc2::FunctionalCommand(
    [this] { m_drive.ResetEncoders(); },
    [this] { m_drive.ArcadeDrive(0.5, 0); },
    [this] (bool interrupted) { m_drive.ArcadeDrive(0, 0); },
    [this, dist] { return m_drive.GetAverageDistance() >= dist; },
    {&m_drive}
  );
}
