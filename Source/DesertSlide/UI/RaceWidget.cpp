// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceWidget.h"

void URaceWidget::Setup()
{
	this->AddToViewport();
}

void URaceWidget::Teardown()
{
	this->RemoveFromParent();
}